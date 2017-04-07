/****************************************************************************
** Meta object code from reading C++ file 'stndev.h'
**
** Created: Thu Jan 15 10:25:24 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/stndev.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stndev.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CStnDevTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      28,   26,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   13,   13,   13, 0x08,
      60,   13,   13,   13, 0x08,
      85,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CStnDevTable[] = {
    "CStnDevTable\0\0update(int)\0,\0update(int,int)\0"
    "displayStnDev()\0select(QTreeWidgetItem*)\0"
    "setLanguage(int)\0"
};

const QMetaObject CStnDevTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CStnDevTable,
      qt_meta_data_CStnDevTable, 0 }
};

const QMetaObject *CStnDevTable::metaObject() const
{
    return &staticMetaObject;
}

void *CStnDevTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CStnDevTable))
        return static_cast<void*>(const_cast< CStnDevTable*>(this));
    if (!strcmp(_clname, "Ui_CStnDevTable"))
        return static_cast< Ui_CStnDevTable*>(const_cast< CStnDevTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CStnDevTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: update((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: update((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: displayStnDev(); break;
        case 3: select((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 4: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CStnDevTable::update(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CStnDevTable::update(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
