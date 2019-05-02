#pragma once

#include "platform.h"
#include <functional>
#include "xo/utility/pointer_types.h"
#include "scone/measures/Measure.h"

namespace scone
{
	class SCONE_LUA_API ScriptMeasure : public Measure
	{
	public:
		ScriptMeasure( const PropNode& props, Params& par, Model& model, const Location& loc );
		virtual ~ScriptMeasure();
		
		virtual double ComputeResult( Model& model ) override;
		virtual bool UpdateMeasure( const Model& model, double timestamp ) override;

	protected:

		virtual String GetClassSignature() const override;

	private:
		u_ptr< class LuaScript > script_;
		std::function<void( struct lua_model* )> init_;
		std::function<bool( struct lua_model* )> update_;
		std::function<double( struct lua_model* )> result_;
	};
}
