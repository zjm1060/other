/****************************************************************************
** Meta object code from reading C++ file 'about.h'
**
** Created: Fri Jan 23 13:44:27 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/about.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'about.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAboutTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_CAboutTable[] = {
    "CAboutTable\0"
};

const QMetaObject CAboutTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CAboutTable,
      qt_meta_data_CAboutTable, 0 }
};

const QMetaObject *CAboutTable::metaObject() const
{
    return &staticMetaObject;
}

void *CAboutTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAboutTable))
        return static_cast<void*>(const_cast< CAboutTable*>(this));
    if (!strcmp(_clname, "Ui_CAboutTable"))
        return static_cast< Ui_CAboutTable*>(const_cast< CAboutTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CAboutTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
