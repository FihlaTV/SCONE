#pragma once

#include "sim.h"
#include "../core/core.h"

#include <vector>
#include "Actuator.h"
#include "Sensors.h"

namespace scone
{
	namespace sim
	{
		class SCONE_SIM_API Muscle : public Actuator
		{
		public:
			Muscle();
			virtual ~Muscle();

			virtual const Link& GetOriginLink() const = 0;
			virtual const Link& GetInsertionLink() const = 0;

			virtual Real GetMomentArm( const Dof& dof ) const = 0;

			virtual Real GetMaxIsometricForce() const = 0;
			virtual Real GetOptimalFiberLength() const = 0;
			virtual Real GetTendonSlackLength() const = 0;
			virtual Real GetMass() const = 0;

			virtual Real GetForce() const = 0;
			virtual Real GetNormalizedForce() const = 0;

			virtual Real GetLength() const = 0;
			virtual Real GetVelocity() const = 0;

			virtual Real GetFiberForce() const = 0;
			virtual Real GetNormalizedFiberForce() const = 0;

			virtual Real GetFiberLength() const = 0;
			virtual Real GetNormalizedFiberLength() const = 0;

			virtual Real GetFiberVelocity() const = 0;
			virtual Real GetNormalizedFiberVelocity() const = 0;

			virtual Real GetTendonLength() const = 0;
			virtual std::vector< Vec3 > GetMusclePath() const = 0;

			virtual Real GetActivation() const = 0;
			virtual Real GetExcitation() const = 0;
			virtual void SetExcitation( Real u ) = 0;

			// checks if a muscle crosses a Dof. Default implementation
			virtual bool HasMomentArm( const Dof& dof ) const;
		};
	}
}
