#ifndef CXXTESTCONFIGURATION_H
#define CXXTESTCONFIGURATION_H

#include "../testconfiguration.h"

namespace Autotest {
	namespace Internal {

		class CxxTestConfiguration : public DebuggableTestConfiguration
		{
		public:
			explicit CxxTestConfiguration(const QString &test =  QString(),
						      const QString &method = QString());

			TestOutputReader *outputReader(const QFutureInterface<TestResultPtr> &fi, QProcess *app) const override;
			QStringList argumentsForTestRunner(QStringList *omitted) const override;
			Utils::Environment filteredEnvironment(const Utils::Environment &original) const override;
		private:
			QString test, method;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTCONFIGURATION_H
