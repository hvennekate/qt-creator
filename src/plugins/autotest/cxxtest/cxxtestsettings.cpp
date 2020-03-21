#include "cxxtestsettings.h"

namespace Autotest {
	namespace Internal {
		QString CxxTestSettings::name() const
		{
			return "CxxTest";
		}

		void CxxTestSettings::toFrameworkSettings(QSettings * /* s */) const
		{}

		void CxxTestSettings::fromFrameworkSettings(const QSettings * /* s */)
		{}
	} // namespace Internal
} // namespace Autotest
