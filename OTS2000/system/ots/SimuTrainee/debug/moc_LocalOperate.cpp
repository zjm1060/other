/****************************************************************************
** Meta object code from reading C++ file 'LocalOperate.h'
**
** Created: Thu Jan 22 17:27:45 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/LocalOperate.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LocalOperate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_COptPrevWidget[] = {

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

static const char qt_meta_stringdata_COptPrevWidget[] = {
    "COptPrevWidget\0"
};

const QMetaObject COptPrevWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_COptPrevWidget,
      qt_meta_data_COptPrevWidget, 0 }
};

const QMetaObject *COptPrevWidget::metaObject() const
{
    return &staticMetaObject;
}

void *COptPrevWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_COptPrevWidget))
        return static_cast<void*>(const_cast< COptPrevWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int COptPrevWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_COptSelectWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x0a,
      49,   44,   17,   17, 0x0a,
      77,   17,   65,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_COptSelectWidget[] = {
    "COptSelectWidget\0\0,name\0addOpt(int,QString)\0"
    "name\0delOpt(QString)\0QStringList\0"
    "getAllOpt()\0"
};

const QMetaObject COptSelectWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_COptSelectWidget,
      qt_meta_data_COptSelectWidget, 0 }
};

const QMetaObject *COptSelectWidget::metaObject() const
{
    return &staticMetaObject;
}

void *COptSelectWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_COptSelectWidget))
        return static_cast<void*>(const_cast< COptSelectWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int COptSelectWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addOpt((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: delOpt((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: { QStringList _r = getAllOpt();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_COptImageWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      25,   17,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_COptImageWidget[] = {
    "COptImageWidget\0\0optName\0optSelected(QString)\0"
};

const QMetaObject COptImageWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_COptImageWidget,
      qt_meta_data_COptImageWidget, 0 }
};

const QMetaObject *COptImageWidget::metaObject() const
{
    return &staticMetaObject;
}

void *COptImageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_COptImageWidget))
        return static_cast<void*>(const_cast< COptImageWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int COptImageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: optSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void COptImageWidget::optSelected(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_COptWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      37,   12,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_COptWidget[] = {
    "COptWidget\0\0opt_type,opt_id,opt_name\0"
    "optSelected(int,int,QString)\0"
};

const QMetaObject COptWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_COptWidget,
      qt_meta_data_COptWidget, 0 }
};

const QMetaObject *COptWidget::metaObject() const
{
    return &staticMetaObject;
}

void *COptWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_COptWidget))
        return static_cast<void*>(const_cast< COptWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int COptWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: optSelected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void COptWidget::optSelected(int _t1, int _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_CLocalOprate[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   25,   13,   13, 0x08,
      78,   13,   13,   13, 0x08,
      87,   13,   13,   13, 0x08,
      99,   13,   13,   13, 0x08,
     115,  109,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CLocalOprate[] = {
    "CLocalOprate\0\0closedlg()\0"
    "opt_type,opt_id,opt_name\0"
    "optPreview(int,int,QString)\0optRun()\0"
    "optFilter()\0onclose()\0index\0"
    "on_currentChanged(int)\0"
};

const QMetaObject CLocalOprate::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CLocalOprate,
      qt_meta_data_CLocalOprate, 0 }
};

const QMetaObject *CLocalOprate::metaObject() const
{
    return &staticMetaObject;
}

void *CLocalOprate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CLocalOprate))
        return static_cast<void*>(const_cast< CLocalOprate*>(this));
    return QWidget::qt_metacast(_clname);
}

int CLocalOprate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: closedlg(); break;
        case 1: optPreview((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: optRun(); break;
        case 3: optFilter(); break;
        case 4: onclose(); break;
        case 5: on_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CLocalOprate::closedlg()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
