#pragma once

#include "Measure.h"
#include "../sim/Body.h"
#include "../core/SampledValue.h"

namespace scone
{
	namespace cs
	{
		class HeightMeasure : public Measure
		{
		public:
			HeightMeasure( const PropNode& props, opt::ParamSet& par, sim::Model& model, const sim::Area& area );
			virtual ~HeightMeasure() { };

			virtual void UpdateControls( sim::Model& model, double timestamp ) override;
			virtual double GetResult( sim::Model& model ) override;

			virtual String GetSignature() override;

		private:
			String target_body;
			bool use_average_height;
			bool terminate_on_peak;
			double termination_height;

			sim::Body* m_pTargetBody; // non-owning pointer
			SampledValue< double > m_Height;
			bool m_Upward;
		};
	}
}