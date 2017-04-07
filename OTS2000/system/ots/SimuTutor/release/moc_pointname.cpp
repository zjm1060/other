/****************************************************************************
** Meta object code from reading C++ file 'pointname.h'
**
** Created: Thu Jan 15 10:27:35 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/pointname.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pointname.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPointNameTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      43,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      69,   16,   16,   16, 0x08,
     100,   16,   16,   16, 0x08,
     108,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPointNameTable[] = {
    "CPointNameTable\0\0sig_curPointName(QString)\0"
    "sig_curEntryName(QString)\0"
    "ItemSelected(QTreeWidgetItem*)\0Enter()\0"
    "setLanguage(int)\0"
};

const QMetaObject CPointNameTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CPointNameTable,
      qt_meta_data_CPointNameTable, 0 }
};

const QMetaObject *CPointNameTable::metaObject() const
{
    return &staticMetaObject;
}

void *CPointNameTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPointNameTable))
        return static_cast<void*>(const_cast< CPointNameTable*>(this));
    if (!strcmp(_clname, "Ui_CPointNameTable"))
        return static_cast< Ui_CPointNameTable*>(const_cast< CPointNameTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CPointNameTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sig_curPointName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: sig_curEntryName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: ItemSelected((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 3: Enter(); break;
        case 4: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CPointNameTable::sig_curPointName(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPointNameTable::sig_curEntryName(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
