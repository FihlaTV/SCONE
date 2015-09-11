#include "stdafx.h"

#include "MetaReflexMuscle.h"
#include "../sim/Model.h"
#include "../sim/SensorDelayAdapter.h"
#include "../sim/Dof.h"
#include "MetaReflexDof.h"
#include "boost/foreach.hpp"
#include "MetaReflexController.h"
#include "tools.h"

//#define DEBUG_MUSCLE "iliopsoas_r"
#define INFO_MUSCLE "iliopsoas_r"

namespace scone
{
	namespace cs
	{
		MetaReflexMuscle::MetaReflexMuscle( sim::Muscle& mus, sim::Model& model, const MetaReflexController& controller, const sim::Area& area ) : 
		muscle( mus ),
		force_sensor( model.AcquireDelayedSensor< sim::MuscleForceSensor >( mus ) ),
		length_sensor( model.AcquireDelayedSensor< sim::MuscleLengthSensor >( mus ) ),
		length_gain( 0.0 ),
		constant( 0.0 ),
		force_gain( 0.0 ),
		delay( 0.0 ),
		stiffness( 0.0 ),
		total_abs_moment_arm( 0.0 )
		{
			base_length = ( muscle.GetLength() - muscle.GetTendonSlackLength() ) / muscle.GetOptimalFiberLength();

			// precompute number of dofs and total moment arm
			BOOST_FOREACH( const MetaReflexDofUP& mrdof, controller.GetReflexDofs() )
			{
				if ( muscle.HasMomentArm( mrdof->target_dof ) )
				{
					DofInfo di( *mrdof );

					di.moment_arm = muscle.GetMomentArm( mrdof->target_dof );
					total_abs_moment_arm += abs( di.moment_arm );

					di.dof.target_dof.SetVel( Radian( Degree( 1 ) ) );
					di.lengthening_speed = muscle.GetVelocity();
					di.dof.target_dof.SetVel( Radian( 0 ) );

					dof_infos.push_back( di );
				}
			}

			// now compute the max available moment for each dof
			BOOST_FOREACH( DofInfo& di, dof_infos )
			{
				di.w = di.moment_arm / total_abs_moment_arm;
				di.abs_w = abs( di.w );
				di.max_moment = di.abs_w * di.moment_arm * muscle.GetMaxIsometricForce();
				di.dof.AddAvailableMoment( di.max_moment );
			}
		}

		MetaReflexMuscle::~MetaReflexMuscle()
		{
		}

		void MetaReflexMuscle::UpdateMuscleControlParameters()
		{
			// initialize reference length
			ref_length = base_length;
			length_gain = 0;
			constant = 0;
			force_gain = 0;

			// compute muscle feedback parameters
			BOOST_FOREACH( const DofInfo& di, dof_infos )
			{
				ref_length += di.w * di.lengthening_speed * di.dof.bal_par.ref_pos * di.dof.GetLocalBalance();

				length_gain += di.abs_w * di.dof.dof_par.length_gain;
				length_gain += di.abs_w * di.dof.bal_par.length_gain * di.dof.GetLocalBalance();

				force_gain += di.w * di.dof.dof_par.force_gain;
				force_gain += di.w * di.dof.bal_par.force_gain * di.dof.GetLocalBalance();

				constant += di.w * di.dof.dof_par.constant;
				constant += di.w * di.dof.bal_par.constant * di.dof.GetLocalBalance();

				// stiffness
				//if ( di.dof.dof_par.stiffness > 0.0 )
				//	mus_par.stiffness += ComputeStiffnessExcitation( *di.dof );

				// delay, average of all MetaMuscleDofs
				delay += ( 1.0 / dof_infos.size() ) * di.dof.delay; // TODO: compute per muscle

#ifdef INFO_MUSCLE
				if ( muscle.GetName() == INFO_MUSCLE || strlen( INFO_MUSCLE ) == 0 )
				{
					Real lb = di.dof.GetLocalBalance();
					Real bc = di.dof.bal_par.constant * di.dof.GetLocalBalance();
					log::TraceF( "%-20s%-20sref=%6.3f w=%5.2f lb=%.3f bc=%.3f",
						muscle.GetName().c_str(), di.dof.target_dof.GetName().c_str(), 
						ref_length, di.w, lb, bc );
				}
#endif
			}
		}

		Real MetaReflexMuscle::ComputeStiffnessExcitation( MetaReflexDof& dof )
		{
			Real mus_mom_arm = muscle.GetMomentArm( dof.target_dof );
			Real max_mus_mom = mus_mom_arm * muscle.GetMaxIsometricForce();

			Real max_abs_dof_mom = std::min( abs( dof.tot_available_neg_mom ), dof.tot_available_pos_mom );
			Real des_dof_mom = dof.dof_par.stiffness * Sign( mus_mom_arm ) * max_abs_dof_mom;

			Real max_stiffness_mom = ( mus_mom_arm < 0 ) ? dof.tot_available_neg_mom : dof.tot_available_pos_mom;
			Real available_mus_mom = ( abs( mus_mom_arm ) / total_abs_moment_arm ) * max_mus_mom;
			Real mus_mom_contrib = available_mus_mom / max_stiffness_mom;

			Real des_mus_mom = mus_mom_contrib * des_dof_mom;
			Real a = des_mus_mom / max_mus_mom;

			return a;
		}

		void MetaReflexMuscle::UpdateControls()
		{
			// length feedback
			Real ul = length_gain * std::max( 0.0, length_sensor.GetValue( delay ) - base_length );

			// constant excitation
			Real uc = std::max( 0.0, constant + stiffness );

			// force feedback
			Real uf = force_gain * std::max( 0.0, force_sensor.GetValue( delay ) );

			// compute total
			Real total = ul + uc + uf;

			muscle.AddControlValue( total );

#ifdef DEBUG_MUSCLE
			if ( muscle.GetName() == DEBUG_MUSCLE )
				log::TraceF( "%s: l=%.3f ref_l=%.3f lg=%.3f ul=%.3f", DEBUG_MUSCLE, length_sensor.GetValue( delay ), base_length, length_gain, ul );
#endif
		}
	}
}
