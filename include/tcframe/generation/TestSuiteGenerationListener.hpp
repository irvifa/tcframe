#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "tcframe/generation/TestCaseGenerationResult.hpp"
#include "tcframe/generation/VerificationFailure.hpp"
#include "tcframe/verification/ConstraintSuiteVerificationResult.hpp"

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::vector;

namespace tcframe { namespace experimental {

class TestSuiteGenerationListener {
public:
    virtual ~TestSuiteGenerationListener() {}

    void onIntroduction() {
        cout << "Generating test cases..." << endl;
    }

    void onTestGroupIntroduction(int testGroupId) {
        cout << endl;

        if (testGroupId == 0) {
            cout << "[ SAMPLE TEST CASES ]" << endl;
        } else if (testGroupId == -1) {
            cout << "[ OFFICIAL TEST CASES ]" << endl;
        } else {
            cout << "[ TEST GROUP " << testGroupId << " ]" << endl;
        }
    }

    void onTestCaseIntroduction(string testCaseName) {
        cout << "  " << testCaseName << ": " << flush;
    }

    void onTestCaseGenerationResult(const string& constraintDescription, const TestCaseGenerationResult& result) {
        if (result.isSuccessful()) {
            cout << "OK" << endl;
        } else {
            cout << "FAILED" << endl;
            cout << "    Description: " << constraintDescription << endl;
            cout << "    Reasons:" << endl;

            printTestCaseGenerationFailure(result.failure());
        }
    }

private:
    void printTestCaseGenerationFailure(TestCaseGenerationFailure* failure) {
        // TODO: use visitor pattern?
        if (VerificationFailure* verificationFailure = dynamic_cast<VerificationFailure*>(failure)) {
            printVerificationFailure(verificationFailure->verificationResult());
        }
    }

    void printVerificationFailure(const ConstraintSuiteVerificationResult& result) {
        for (const auto& entry : result.unsatisfiedConstraintDescriptionsByConstraintGroupId()) {
            int constraintGroupId = entry.first;
            const vector<string>& unsatisfiedConstraintDescriptions = entry.second;

            if (constraintGroupId == -1) {
                cout << "    * Does not satisfy constraints, on:" << endl;
            } else {
                cout << "    * Does not satisfy subtask " << constraintGroupId << ", on constraints:" << endl;
            }

            for (const string& unsatisfiedConstraintDescription : unsatisfiedConstraintDescriptions) {
                cout << "       - " << unsatisfiedConstraintDescription << endl;
            }
        }
        for (int constraintGroupId : result.satisfiedButNotAssignedConstraintGroupIds()) {
            cout << "    * Satisfies subtask " << constraintGroupId << " but is not assigned to it" << endl;
        }
    }
 };

}}
