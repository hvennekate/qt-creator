#include "cxxtestsettings.h"

namespace Autotest {
	namespace Internal {
		QString CxxTestSettings::name() const
		{
			return "CxxTest";
		}

		void CxxTestSettings::toFrameworkSettings(QSettings *s) const
		{
			Q_UNUSED(s);
		}

		void CxxTestSettings::fromFrameworkSettings(const QSettings *s)
		{
			Q_UNUSED(s);
		}
	} // namespace Internal
} // namespace Autotest
