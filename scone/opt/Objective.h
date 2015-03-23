#pragma once

#include "opt.h"
#include "ParamSet.h"
#include "../core/InitFromPropNode.h"

namespace scone
{
	namespace opt
	{
		class OPT_API Objective : public Parameterizable
		{
		public:
			Objective( const PropNode& props, ParamSet& par );
			virtual ~Objective();

			// get all parameters in this objective
			ParamSet GetParamSet();

			// update all parameters and call Evaluate
			double Evaluate( ParamSet& par );
			virtual void WriteResults( const String& file ) { };

			// virtual evaluation function
			virtual double Evaluate() = 0;

			// a signature describing the objective
			virtual String GetSignature() { return ""; }

			int debug_idx;
		};
	}
}
