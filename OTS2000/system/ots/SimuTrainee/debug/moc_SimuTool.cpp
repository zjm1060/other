/****************************************************************************
** Meta object code from reading C++ file 'SimuTool.h'
**
** Created: Thu Jan 22 17:27:25 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/SimuTool.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SimuTool.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SimuTool[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      37,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      36,    9,    9,    9, 0x05,
      60,    9,    9,    9, 0x05,
      87,    9,    9,    9, 0x05,
     111,    9,    9,    9, 0x05,
     133,    9,    9,    9, 0x05,
     157,    9,    9,    9, 0x05,
     186,    9,    9,    9, 0x05,
     211,    9,    9,    9, 0x05,
     233,    9,    9,    9, 0x05,
     262,    9,    9,    9, 0x05,
     288,    9,    9,    9, 0x05,
     314,    9,    9,    9, 0x05,
     343,    9,    9,    9, 0x05,
     371,    9,    9,    9, 0x05,
     399,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     421,    9,    9,    9, 0x08,
     442,    9,    9,    9, 0x08,
     461,    9,    9,    9, 0x08,
     483,    9,    9,    9, 0x08,
     502,    9,    9,    9, 0x08,
     519,    9,    9,    9, 0x08,
     538,    9,    9,    9, 0x08,
     562,    9,    9,    9, 0x08,
     582,    9,    9,    9, 0x08,
     599,    9,    9,    9, 0x08,
     623,    9,    9,    9, 0x08,
     644,    9,    9,    9, 0x08,
     665,    9,    9,    9, 0x08,
     689,    9,    9,    9, 0x08,
     712,    9,    9,    9, 0x08,
     735,    9,    9,    9, 0x08,
     752,    9,    9,    9, 0x08,
     766,    9,    9,    9, 0x08,
     785,  781,    9,    9, 0x08,
     811,  805,    9,    9, 0x08,
     827,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SimuTool[] = {
    "SimuTool\0\0button_userinfo_clicked()\0"
    "button_record_clicked()\0"
    "button_userscore_clicked()\0"
    "button_logout_clicked()\0button_task_clicked()\0"
    "button_revert_clicked()\0"
    "button_localOprate_clicked()\0"
    "button_openOix_clicked()\0button_Exam_clicked()\0"
    "button_simuControl_clicked()\0"
    "button_setScene_clicked()\0"
    "button_setFault_clicked()\0"
    "button_sendMessage_clicked()\0"
    "button_doccuments_clicked()\0"
    "button_setDesplay_clicked()\0"
    "button_help_clicked()\0on_button_userinfo()\0"
    "on_button_record()\0on_button_userscore()\0"
    "on_button_logout()\0on_button_task()\0"
    "on_button_revert()\0on_button_localOprate()\0"
    "on_button_openOix()\0on_button_Exam()\0"
    "on_button_simuControl()\0on_button_setScene()\0"
    "on_button_setFault()\0on_button_sendMessage()\0"
    "on_button_doccuments()\0on_button_setDesplay()\0"
    "on_button_help()\0on_act_hide()\0"
    "on_act_close()\0pos\0on_menuList(QPoint)\0"
    "index\0MysetIndex(int)\0setLanguage(int)\0"
};

const QMetaObject SimuTool::staticMetaObject = {
    { &QToolBox::staticMetaObject, qt_meta_stringdata_SimuTool,
      qt_meta_data_SimuTool, 0 }
};

const QMetaObject *SimuTool::metaObject() const
{
    return &staticMetaObject;
}

void *SimuTool::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimuTool))
        return static_cast<void*>(const_cast< SimuTool*>(this));
    if (!strcmp(_clname, "Ui_Simutool"))
        return static_cast< Ui_Simutool*>(const_cast< SimuTool*>(this));
    return QToolBox::qt_metacast(_clname);
}

int SimuTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: button_userinfo_clicked(); break;
        case 1: button_record_clicked(); break;
        case 2: button_userscore_clicked(); break;
        case 3: button_logout_clicked(); break;
        case 4: button_task_clicked(); break;
        case 5: button_revert_clicked(); break;
        case 6: button_localOprate_clicked(); break;
        case 7: button_openOix_clicked(); break;
        case 8: button_Exam_clicked(); break;
        case 9: button_simuControl_clicked(); break;
        case 10: button_setScene_clicked(); break;
        case 11: button_setFault_clicked(); break;
        case 12: button_sendMessage_clicked(); break;
        case 13: button_doccuments_clicked(); break;
        case 14: button_setDesplay_clicked(); break;
        case 15: button_help_clicked(); break;
        case 16: on_button_userinfo(); break;
        case 17: on_button_record(); break;
        case 18: on_button_userscore(); break;
        case 19: on_button_logout(); break;
        case 20: on_button_task(); break;
        case 21: on_button_revert(); break;
        case 22: on_button_localOprate(); break;
        case 23: on_button_openOix(); break;
        case 24: on_button_Exam(); break;
        case 25: on_button_simuControl(); break;
        case 26: on_button_setScene(); break;
        case 27: on_button_setFault(); break;
        case 28: on_button_sendMessage(); break;
        case 29: on_button_doccuments(); break;
        case 30: on_button_setDesplay(); break;
        case 31: on_button_help(); break;
        case 32: on_act_hide(); break;
        case 33: on_act_close(); break;
        case 34: on_menuList((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 35: MysetIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 37;
    }
    return _id;
}

// SIGNAL 0
void SimuTool::button_userinfo_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SimuTool::button_record_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SimuTool::button_userscore_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void SimuTool::button_logout_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void SimuTool::button_task_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void SimuTool::button_revert_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void SimuTool::button_localOprate_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void SimuTool::button_openOix_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void SimuTool::button_Exam_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void SimuTool::button_simuControl_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void SimuTool::button_setScene_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void SimuTool::button_setFault_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void SimuTool::button_sendMessage_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void SimuTool::button_doccuments_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void SimuTool::button_setDesplay_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void SimuTool::button_help_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}
QT_END_MOC_NAMESPACE
