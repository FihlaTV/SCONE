#include "StudioSettings.h"
#include "simvis/color.h"
#include "xo/filesystem/path.h"

namespace scone
{
	xo::settings g_StudioSettings;
	xo::settings GetStudioSettings()
	{
		auto& s = g_StudioSettings;
		if ( s.empty() )
		{
			s.add( "viewer.background", "Viewer Background", vis::make_from_hex( 0x808080 ) );
			s.add( "viewer.tile1", "Viewer Tile A", vis::make_from_hsv( 215.0f, 0.0f, 0.52f ) );
			s.add( "viewer.tile2", "Viewer Tile B", vis::make_from_hsv( 215.0f, 0.0f, 0.48f ) );
			s.add( "viewer.bone", "Bone color", vis::make_from_hsv( 35.0f, 0.25f, 1.0f ) );
			s.add( "viewer.tendon", "Tendon color", vis::make_from_hsv( 35.0f, 0.02f, 1.0f ) );
			s.add( "viewer.forces", "Force color", vis::make_from_hsv( 60.0f, 0.75f, 1.0f ) );
			s.add( "viewer.contact", "Contact geometry color", vis::make_from_hsv( 180.0f, 0.75f, 0.75f ) );
			s.add( "tools.path_to_ffmpeg", "Path to ffmpeg.exe, required for generating videos", xo::path() );
		}

		return s;
	}
}
