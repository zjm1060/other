/****************************************************************************
** Meta object code from reading C++ file 'Simucontrol.h'
**
** Created: Thu Jan 22 17:27:29 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Simucontrol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Simucontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Simucontrol[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   13,   12,   12, 0x05,
      42,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   12,   12,   12, 0x08,
      65,   12,   12,   12, 0x08,
      82,   76,   12,   12, 0x08,
     105,   12,   12,   12, 0x08,
     120,   12,   12,   12, 0x08,
     139,   12,   12,   12, 0x08,
     149,   12,   12,   12, 0x08,
     156,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Simucontrol[] = {
    "Simucontrol\0\0str\0sigSpeedChanged(QString)\0"
    "closedlg()\0simuStart()\0simuStop()\0"
    "value\0setSimuSpeedLabel(int)\0"
    "setSimuSpeed()\0setSimuSpeedMenu()\0"
    "onclose()\0onok()\0setLanguage(int)\0"
};

const QMetaObject Simucontrol::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Simucontrol,
      qt_meta_data_Simucontrol, 0 }
};

const QMetaObject *Simucontrol::metaObject() const
{
    return &staticMetaObject;
}

void *Simucontrol::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Simucontrol))
        return static_cast<void*>(const_cast< Simucontrol*>(this));
    if (!strcmp(_clname, "Ui_Simucontrol"))
        return static_cast< Ui_Simucontrol*>(const_cast< Simucontrol*>(this));
    return QWidget::qt_metacast(_clname);
}

int Simucontrol::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigSpeedChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: closedlg(); break;
        case 2: simuStart(); break;
        case 3: simuStop(); break;
        case 4: setSimuSpeedLabel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setSimuSpeed(); break;
        case 6: setSimuSpeedMenu(); break;
        case 7: onclose(); break;
        case 8: onok(); break;
        case 9: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Simucontrol::sigSpeedChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Simucontrol::closedlg()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
