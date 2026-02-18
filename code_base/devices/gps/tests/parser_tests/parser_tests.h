#ifndef DEVICES_GPS_TESTS_PARSER_TESTS_PARSER_TESTS_H_
#define DEVICES_GPS_TESTS_PARSER_TESTS_PARSER_TESTS_H_

#include <QObject>
#include <QtTest>

namespace mock {

extern const char GGA_VALID[];
extern const char RMC_VALID[];

extern const char GGA_BROKEN[];
extern const char RMC_BROKEN[];

}  // namespace mock

class TestsParser : public QObject {
    Q_OBJECT
public:
    TestsParser();

private slots:
    void test_gga_rmc_pair();
    void test_broken_lines();
    void test_unreal_values();
};

#endif  // DEVICES_GPS_TESTS_PARSER_TESTS_PARSER_TESTS_H_
