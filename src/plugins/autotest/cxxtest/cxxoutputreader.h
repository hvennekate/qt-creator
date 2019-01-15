#ifndef CXXOUTPUTREADER_H
#define CXXOUTPUTREADER_H

#include "../testoutputreader.h"

#include <QStack>

namespace Autotest {
	namespace Internal {

		class CxxOutputReader : public TestOutputReader
		{
		public:
			CxxOutputReader(const QFutureInterface<TestResultPtr> &futureInterface,
					QProcess *testApplication, const QString &buildDirectory,
					const QString &projectFile);
		protected:
			void processOutputLine(const QByteArray &outputLineWithNewLine) override;
			TestResultPtr createDefaultResult() const override;
		private:
			QStack<const TestResult*> parents;
			const TestResult *currentParent() const;
			QString project;
			QString worldName;
			QString suiteName;
			QString testName;
			static long processCounter;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXOUTPUTREADER_H
