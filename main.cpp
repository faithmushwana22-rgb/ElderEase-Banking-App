#include "eldereaseapp.h"
#include "SignupPage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElderEaseApp window;
    window.show();

    return a.exec();
}
