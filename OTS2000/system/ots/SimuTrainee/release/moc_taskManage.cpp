/****************************************************************************
** Meta object code from reading C++ file 'taskManage.h'
**
** Created: Wed Jan 14 10:22:12 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/taskManage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'taskManage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CalcScoreThread[] = {

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

static const char qt_meta_stringdata_CalcScoreThread[] = {
    "CalcScoreThread\0"
};

const QMetaObject CalcScoreThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CalcScoreThread,
      qt_meta_data_CalcScoreThread, 0 }
};

const QMetaObject *CalcScoreThread::metaObject() const
{
    return &staticMetaObject;
}

void *CalcScoreThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CalcScoreThread))
        return static_cast<void*>(const_cast< CalcScoreThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CalcScoreThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_CScore[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      25,    7,    7,    7, 0x08,
      44,    7,    7,    7, 0x08,
      63,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CScore[] = {
    "CScore\0\0showWaitDialog()\0showResultDialog()\0"
    "showDetailDialog()\0timeOut()\0"
};

const QMetaObject CScore::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CScore,
      qt_meta_data_CScore, 0 }
};

const QMetaObject *CScore::metaObject() const
{
    return &staticMetaObject;
}

void *CScore::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CScore))
        return static_cast<void*>(const_cast< CScore*>(this));
    return QDialog::qt_metacast(_clname);
}

int CScore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showWaitDialog(); break;
        case 1: showResultDialog(); break;
        case 2: showDetailDialog(); break;
        case 3: timeOut(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
static const uint qt_meta_data_CTaskManage[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      39,   12,   12,   12, 0x05,
      52,   12,   12,   12, 0x05,
      69,   12,   12,   12, 0x05,
      87,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      98,   12,   12,   12, 0x0a,
     110,   12,   12,   12, 0x0a,
     134,  124,   12,   12, 0x0a,
     161,  151,   12,   12, 0x0a,
     181,  151,   12,   12, 0x0a,
     217,  208,   12,   12, 0x0a,
     234,   12,   12,   12, 0x08,
     244,   12,   12,   12, 0x08,
     260,   12,   12,   12, 0x08,
     273,   12,   12,   12, 0x08,
     293,   12,   12,   12, 0x08,
     303,   12,   12,   12, 0x08,
     327,  321,   12,   12, 0x08,
     346,   12,   12,   12, 0x08,
     372,  366,   12,   12, 0x08,
     404,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CTaskManage[] = {
    "CTaskManage\0\0sigTaskTimeChanged(QTime)\0"
    "sigTimeOut()\0sigTaskStarted()\0"
    "sigTaskFinished()\0closedlg()\0taskStart()\0"
    "taskReStart()\0isUnusual\0taskFinish(bool)\0"
    "task_info\0addTask(TASK_INFO*)\0"
    "setCurTaskInfo(TASK_INFO*)\0language\0"
    "setLanguage(int)\0delTask()\0clearTaskList()\0"
    "setCurTask()\0taskTimeCountDown()\0"
    "timeOut()\0saveTaskToExcel()\0state\0"
    "switchSimuTip(int)\0displayTaskRec(int)\0"
    "index\0PopupTaskRecDetail(QModelIndex)\0"
    "onexit()\0"
};

const QMetaObject CTaskManage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTaskManage,
      qt_meta_data_CTaskManage, 0 }
};

const QMetaObject *CTaskManage::metaObject() const
{
    return &staticMetaObject;
}

void *CTaskManage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTaskManage))
        return static_cast<void*>(const_cast< CTaskManage*>(this));
    if (!strcmp(_clname, "Ui_CTaskManage"))
        return static_cast< Ui_CTaskManage*>(const_cast< CTaskManage*>(this));
    return QWidget::qt_metacast(_clname);
}

int CTaskManage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigTaskTimeChanged((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 1: sigTimeOut(); break;
        case 2: sigTaskStarted(); break;
        case 3: sigTaskFinished(); break;
        case 4: closedlg(); break;
        case 5: taskStart(); break;
        case 6: taskReStart(); break;
        case 7: taskFinish((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: addTask((*reinterpret_cast< TASK_INFO*(*)>(_a[1]))); break;
        case 9: setCurTaskInfo((*reinterpret_cast< TASK_INFO*(*)>(_a[1]))); break;
        case 10: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: delTask(); break;
        case 12: clearTaskList(); break;
        case 13: setCurTask(); break;
        case 14: taskTimeCountDown(); break;
        case 15: timeOut(); break;
        case 16: saveTaskToExcel(); break;
        case 17: switchSimuTip((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: displayTaskRec((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: PopupTaskRecDetail((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 20: onexit(); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void CTaskManage::sigTaskTimeChanged(const QTime & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CTaskManage::sigTimeOut()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CTaskManage::sigTaskStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CTaskManage::sigTaskFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CTaskManage::closedlg()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
static const uint qt_meta_data_CNewTaskTip[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      32,   12,   12,   12, 0x08,
      45,   12,   12,   12, 0x08,
      59,   12,   12,   12, 0x08,
      72,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CNewTaskTip[] = {
    "CNewTaskTip\0\0sig_taskaccepted()\0"
    "setCurTask()\0addTaskList()\0ignoreTask()\0"
    "updateTitle()\0"
};

const QMetaObject CNewTaskTip::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CNewTaskTip,
      qt_meta_data_CNewTaskTip, 0 }
};

const QMetaObject *CNewTaskTip::metaObject() const
{
    return &staticMetaObject;
}

void *CNewTaskTip::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CNewTaskTip))
        return static_cast<void*>(const_cast< CNewTaskTip*>(this));
    return QWidget::qt_metacast(_clname);
}

int CNewTaskTip::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sig_taskaccepted(); break;
        case 1: setCurTask(); break;
        case 2: addTaskList(); break;
        case 3: ignoreTask(); break;
        case 4: updateTitle(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CNewTaskTip::sig_taskaccepted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
