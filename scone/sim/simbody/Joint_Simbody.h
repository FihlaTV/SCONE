#pragma once

#include "sim_simbody.h"
#include "../Joint.h"

namespace OpenSim
{
	class Joint;
}

namespace scone
{
	namespace sim
	{
		class SCONE_SIM_SIMBODY_API Joint_Simbody : public Joint
		{
		public:
			Joint_Simbody( class Model_Simbody& model, OpenSim::Joint& osJoint );
			virtual ~Joint_Simbody();

			virtual const String& GetName() const;

			virtual Vec3 GetPos() const override;

			virtual size_t GetDofCount() const override;
			virtual Real GetDof( size_t index = 0 ) const override;
			virtual const String& GetDofName( size_t index = 0 ) const override;

			class Model_Simbody& m_Model;
			OpenSim::Joint& m_osJoint;
		};
	}
}
