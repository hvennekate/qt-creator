#ifndef CXXTESTSETTINGS_H
#define CXXTESTSETTINGS_H

#include <autotest/iframeworksettings.h>

namespace Autotest {
	namespace Internal {

		class CxxTestSettings : public IFrameworkSettings
		{
		public:
			CxxTestSettings() = default;

			QString name() const override;

		protected:
			void toFrameworkSettings(QSettings *s) const override;
			void fromFrameworkSettings(const QSettings *s) override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTSETTINGS_H
