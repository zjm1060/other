/****************************************************************************
** Meta object code from reading C++ file 'hostpage.h'
**
** Created: Wed Jan 14 10:22:29 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/hostpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hostpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Hostpage[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      22,    9,    9,    9, 0x05,
      34,    9,    9,    9, 0x05,
      46,    9,    9,    9, 0x05,
      58,    9,    9,    9, 0x05,
      71,    9,    9,    9, 0x05,
      89,    9,    9,    9, 0x05,
     112,    9,    9,    9, 0x05,
     129,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     150,    9,    9,    9, 0x0a,
     172,    9,    9,    9, 0x0a,
     194,    9,    9,    9, 0x0a,
     216,    9,    9,    9, 0x0a,
     240,    9,    9,    9, 0x0a,
     263,    9,    9,    9, 0x0a,
     289,    9,    9,    9, 0x0a,
     315,    9,    9,    9, 0x0a,
     343,    9,    9,    9, 0x0a,
     371,    9,    9,    9, 0x0a,
     398,    9,    9,    9, 0x0a,
     407,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Hostpage[] = {
    "Hostpage\0\0STclicked()\0SOclicked()\0"
    "ETclicked()\0EOclicked()\0CPTclicked()\0"
    "sig_showrecords()\0sig_showstudyrecords()\0"
    "sig_showscores()\0sig_showpersoninfo()\0"
    "on_toolButton_ModeS()\0on_toolButton_ModeE()\0"
    "on_toolButton_ModeC()\0on_pushButton_EtStudy()\0"
    "on_pushButton_EtExam()\0on_pushButton_EtComplet()\0"
    "on_pushButton_oprrecord()\0"
    "on_pushButton_studyrecord()\0"
    "on_pushButton_scorerecord()\0"
    "on_toolButton_personinfo()\0update()\0"
    "setLanguage(int)\0"
};

const QMetaObject Hostpage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Hostpage,
      qt_meta_data_Hostpage, 0 }
};

const QMetaObject *Hostpage::metaObject() const
{
    return &staticMetaObject;
}

void *Hostpage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Hostpage))
        return static_cast<void*>(const_cast< Hostpage*>(this));
    if (!strcmp(_clname, "Ui_hostpage"))
        return static_cast< Ui_hostpage*>(const_cast< Hostpage*>(this));
    return QWidget::qt_metacast(_clname);
}

int Hostpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: STclicked(); break;
        case 1: SOclicked(); break;
        case 2: ETclicked(); break;
        case 3: EOclicked(); break;
        case 4: CPTclicked(); break;
        case 5: sig_showrecords(); break;
        case 6: sig_showstudyrecords(); break;
        case 7: sig_showscores(); break;
        case 8: sig_showpersoninfo(); break;
        case 9: on_toolButton_ModeS(); break;
        case 10: on_toolButton_ModeE(); break;
        case 11: on_toolButton_ModeC(); break;
        case 12: on_pushButton_EtStudy(); break;
        case 13: on_pushButton_EtExam(); break;
        case 14: on_pushButton_EtComplet(); break;
        case 15: on_pushButton_oprrecord(); break;
        case 16: on_pushButton_studyrecord(); break;
        case 17: on_pushButton_scorerecord(); break;
        case 18: on_toolButton_personinfo(); break;
        case 19: update(); break;
        case 20: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void Hostpage::STclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Hostpage::SOclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Hostpage::ETclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Hostpage::EOclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Hostpage::CPTclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void Hostpage::sig_showrecords()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void Hostpage::sig_showstudyrecords()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Hostpage::sig_showscores()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void Hostpage::sig_showpersoninfo()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}
QT_END_MOC_NAMESPACE
