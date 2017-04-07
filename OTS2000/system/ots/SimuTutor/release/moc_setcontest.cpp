/****************************************************************************
** Meta object code from reading C++ file 'setcontest.h'
**
** Created: Thu Jan 15 10:27:02 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/setcontest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setcontest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSetContestTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      35,   17,   17,   17, 0x0a,
      53,   17,   17,   17, 0x0a,
      66,   17,   17,   17, 0x0a,
      85,   17,   17,   17, 0x0a,
     107,   17,   17,   17, 0x0a,
     124,   17,   17,   17, 0x0a,
     139,   17,   17,   17, 0x0a,
     152,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSetContestTable[] = {
    "CSetContestTable\0\0sig_tasksetted()\0"
    "setContestModel()\0setContest()\0"
    "setContestFilter()\0setSendContestModel()\0"
    "AddsendContest()\0StartContest()\0"
    "DelContest()\0setLanguage(int)\0"
};

const QMetaObject CSetContestTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSetContestTable,
      qt_meta_data_CSetContestTable, 0 }
};

const QMetaObject *CSetContestTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSetContestTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSetContestTable))
        return static_cast<void*>(const_cast< CSetContestTable*>(this));
    if (!strcmp(_clname, "Ui_CSetContestTable"))
        return static_cast< Ui_CSetContestTable*>(const_cast< CSetContestTable*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSetContestTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sig_tasksetted(); break;
        case 1: setContestModel(); break;
        case 2: setContest(); break;
        case 3: setContestFilter(); break;
        case 4: setSendContestModel(); break;
        case 5: AddsendContest(); break;
        case 6: StartContest(); break;
        case 7: DelContest(); break;
        case 8: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CSetContestTable::sig_tasksetted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
