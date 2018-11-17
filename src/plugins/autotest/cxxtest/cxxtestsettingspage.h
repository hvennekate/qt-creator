#ifndef CXXTESTSETTINGSPAGE_H
#define CXXTESTSETTINGSPAGE_H

#include <autotest/itestsettingspage.h>

namespace Autotest {
	namespace Internal {

		class CxxTestSettingsPagePrivate;
		class CxxTestSettingsPage : public ITestSettingsPage
		{
			Q_DECLARE_PRIVATE(CxxTestSettingsPage)
			QScopedPointer<CxxTestSettingsPagePrivate> d_ptr;
		public:
			CxxTestSettingsPage(QSharedPointer<IFrameworkSettings> settings, const ITestFramework *framework);
			QWidget *widget() override;
			void apply() override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTSETTINGSPAGE_H
