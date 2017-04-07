/****************************************************************************
** Meta object code from reading C++ file 'login.h'
**
** Created: Thu Jan 15 10:27:56 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/login.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CLogin[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      26,    8,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      52,    7,    7,    7, 0x08,
      67,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CLogin[] = {
    "CLogin\0\0user_id,user_name\0"
    "sigUserLogin(int,QString)\0teacherLogin()\0"
    "setLanguage(int)\0"
};

const QMetaObject CLogin::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CLogin,
      qt_meta_data_CLogin, 0 }
};

const QMetaObject *CLogin::metaObject() const
{
    return &staticMetaObject;
}

void *CLogin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CLogin))
        return static_cast<void*>(const_cast< CLogin*>(this));
    if (!strcmp(_clname, "Ui_CLoginTable"))
        return static_cast< Ui_CLoginTable*>(const_cast< CLogin*>(this));
    return QDialog::qt_metacast(_clname);
}

int CLogin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigUserLogin((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: teacherLogin(); break;
        case 2: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CLogin::sigUserLogin(int _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
