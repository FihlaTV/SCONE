#pragma once

#include "sim.h"
#include <vector>
#include "../core/Exception.h"
#include "Types.h"

namespace scone
{
	namespace sim
	{
		/// Simple struct to allow tree hierarchy
		class SCONE_SIM_API Link
		{
		public:
			Link( Body& b, Joint& j, Link* parent = nullptr ) : m_Body( &b ), m_Joint( &j ), m_Parent( parent ), type( UnknownLink ) { };
			Link( Body& b ) : m_Body( &b ), m_Joint( nullptr ) { };

			// serialization, can be used for debug printing
			String ToString( const String& prefix = "" ) const;
			
			// get body
			Body& GetBody() { return *m_Body; }
			const Body& GetBody() const { return *m_Body; }

			// get joint
			bool HasJoint() const { return m_Joint != nullptr; }
			Joint& GetJoint() { return *m_Joint; }
			const Joint& GetJoint() const { return *m_Joint; }

			// get children
			std::vector< LinkUP >& GetChildren() { return m_Children; }
			const std::vector< LinkUP >& GetChildren() const { return m_Children; }
			const Link& GetChild( size_t i ) const { return *m_Children[ i ]; }

			// get parent
			const Link& GetParent() const { return *m_Parent; }

			// find link
			const Link* FindLink( const String& body ) const;

			// link metadata
			LinkType type;

		private:
			Link* m_Parent;
			Body* m_Body;
			Joint* m_Joint;
			std::vector< LinkUP > m_Children;

		private: // noncopyable
			Link( const Link& );
			Link& operator=( const Link& );
		};
	}
}