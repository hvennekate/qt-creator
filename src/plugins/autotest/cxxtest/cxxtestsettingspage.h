#ifndef CXXTESTSETTINGSPAGE_H
#define CXXTESTSETTINGSPAGE_H

#include <coreplugin/dialogs/ioptionspage.h>

namespace Autotest {
	namespace Internal {
		class CxxTestSettings;

		class CxxTestSettingsPage : public Core::IOptionsPage
		{
		public:
			CxxTestSettingsPage(CxxTestSettings* settings, Core::Id settingsId);
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTSETTINGSPAGE_H
