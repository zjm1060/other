Ext.ns('com.bitc.xmis.AskForLeave');
com.bitc.xmis.AskForLeave.DateValid = function(absentForm) {

	// 请假申请--start

	absentForm.getForm().findField('plan_begin_date').on('blur', function(c) {
				com.bitc.xmis.AskForLeave.DateValidAbsentDaysSum(absentForm);
			});
	absentForm.getForm().findField('plan_end_date').on('blur', function(c) {
				com.bitc.xmis.AskForLeave.DateValidAbsentDaysSum(absentForm);
			});
	absentForm.getForm().findField('plan_begin_hour').on('blur', function(c) {
				com.bitc.xmis.AskForLeave.DateValidAbsentDaysSum(absentForm);
			});
	absentForm.getForm().findField('plan_end_hour').on('blur', function(c) {
				com.bitc.xmis.AskForLeave.DateValidAbsentDaysSum(absentForm);
			});
	// 各个时间组件加监听:1、开始时间 、结束时间必须在上班-下班的工作时间区间内； 2、同一天，结束时间大于开始时间；2、一个为00；
	com.bitc.xmis.AskForLeave.DateValidAbsentDaysSum(absentForm);
	// 请假申请--stop

	// 销假申请--start

	absentForm.getForm().findField('actual_begin_date').on('blur', function(c) {
				com.bitc.xmis.AskForLeave.DateValidAbsentDaysSumActual(absentForm);
			});
	absentForm.getForm().findField('actual_end_date').on('blur', function(c) {
				com.bitc.xmis.AskForLeave.DateValidAbsentDaysSumActual(absentForm);
			});
	absentForm.getForm().findField('actual_begin_hour').on('blur', function(c) {
				com.bitc.xmis.AskForLeave.DateValidAbsentDaysSumActual(absentForm);
			});
	absentForm.getForm().findField('actual_end_hour').on('blur', function(c) {
				com.bitc.xmis.AskForLeave.DateValidAbsentDaysSumActual(absentForm);
			});
	// 各个时间组件加监听:1、开始时间 、结束时间必须在上班-下班的工作时间区间内； 2、同一天，结束时间大于开始时间；2、一个为00；
	com.bitc.xmis.AskForLeave.DateValidAbsentDaysSumActual(absentForm);
	// 销假申请--stop
};

com.bitc.xmis.AskForLeave.DateValidAbsentDaysSum = function(absentForm) {
	var plan_begin_date = Ext.util.Format.date(absentForm.getForm()
					.findField("plan_begin_date").getValue(), 'Y-m-d');
	var plan_begin_hour = absentForm.getForm().findField("plan_begin_hour")
			.getValue();

	var plan_end_date = Ext.util.Format.date(absentForm.getForm()
					.findField("plan_end_date").getValue(), 'Y-m-d');
	var plan_end_hour = absentForm.getForm().findField("plan_end_hour")
			.getValue();
	if (plan_begin_date == '' || plan_end_date == '') {
		return;
	}
	if (absentForm.getForm().findField("plan_begin_date").getValue() > absentForm
			.getForm().findField("plan_end_date").getValue()) {
		Ext.Msg.alert('提示', ' 结束时间不能小于开始时间');
		return;
	} else if (absentForm.getForm().findField("plan_begin_date").getValue() == absentForm
			.getForm().findField("plan_end_date").getValue()
			&& plan_begin_hour > plan_end_hour) {
		Ext.Msg.alert('提示', ' 结束时间不能小于开始时间');
		return;
	}
	Ext.Ajax.request({
				url : __cxtPath
						+ '/applyManageMaint.do?method=getAbsentDaysSum',
				method : 'post',
				params : {
					plan_begin_date : plan_begin_date,
					plan_begin_hour : plan_begin_hour,
					plan_end_date : plan_end_date,
					plan_end_hour : plan_end_hour
				},
				success : function(response, action) {
					var result = Ext.decode(response.responseText);
					
					if (result.success) {
							var sumStr = result.attributes.sumStr;
							var inputSeft = absentForm.getForm().findField("plan_sum_hidden").getValue();
							var daycount =0;
							var daynum = Ext.util.Format.substr(sumStr,0,sumStr.indexOf("天"));
							var hournum = Ext.util.Format.substr(sumStr,sumStr.indexOf("天")+1,sumStr.indexOf("小")-sumStr.indexOf("天")-1);
									if(daynum != ""){
										daycount = daynum;
									}
									if(hournum != ""){
										var a = hournum;
										if(hournum == 8)
											daycount = daycount+1;
										var  temp = Ext.util.Format.number(hournum/8,'0.0') ;
										hournum = Ext.util.Format.substr(temp,1,temp.length);
										
										daycount = daycount + hournum ;
										var b = daycount;
									}
									absentForm.getForm().findField("plan_sum")
									.setValue(daycount);
							if(inputSeft !="" && inputSeft!=null){
									
									absentForm.getForm().findField("plan_sum")
									.setValue(inputSeft);
							}
					} else {
						Ext.Msg.alert(result.messageTitle, result.message);
					}

				},
				failure : function(response, action) {
					absentForm.getForm().findField("plan_sum").setValue("出错");
				}
			});
};
com.bitc.xmis.AskForLeave.DateValidAbsentDaysSumActual = function(absentForm) {
	var actual_begin_date = Ext.util.Format.date(absentForm.getForm()
					.findField("actual_begin_date").getValue(), 'Y-m-d');
	var actual_begin_hour = absentForm.getForm().findField("actual_begin_hour")
			.getValue();

	var actual_end_date = Ext.util.Format.date(absentForm.getForm()
					.findField("actual_end_date").getValue(), 'Y-m-d');
	var actual_end_hour = absentForm.getForm().findField("actual_end_hour")
			.getValue();

	if (actual_begin_date == '' || actual_end_date == '') {
		return;
	}
	if (absentForm.getForm().findField("actual_begin_date").getValue() > absentForm
			.getForm().findField("actual_end_date").getValue()) {
		Ext.Msg.alert('提示', ' 结束时间不能小于开始时间');
		return;
	} else if (absentForm.getForm().findField("actual_begin_date").getValue() == absentForm
			.getForm().findField("actual_end_date").getValue()
			&& actual_begin_hour > actual_end_hour) {
		Ext.Msg.alert('提示', ' 结束时间不能小于开始时间');
		return;
	}
	Ext.Ajax.request({
				url : __cxtPath
						+ '/applyManageMaint.do?method=getAbsentDaysSum',
				method : 'post',
				params : {
					plan_begin_date : actual_begin_date,
					plan_begin_hour : actual_begin_hour,
					plan_end_date : actual_end_date,
					plan_end_hour : actual_end_hour
				},
				success : function(response, action) {
					var result = Ext.decode(response.responseText);
					if (result.success) {
							var sumStr = result.attributes.sumStr;
							var inputSeft = absentForm.getForm().findField("actual_sum_hidden").getValue();
							var daycount =0;
							var daynum = Ext.util.Format.substr(sumStr,0,sumStr.indexOf("天"));
							var hournum = Ext.util.Format.substr(sumStr,sumStr.indexOf("天")+1,sumStr.indexOf("小")-sumStr.indexOf("天")-1);
									if(daynum != ""){
										daycount = daynum;
									}
									if(hournum != ""){
										if(hournum == 8)
											daycount = daycount+1;
										var  temp = Ext.util.Format.number(hournum/8,'0.0') ;
										hournum = Ext.util.Format.substr(temp,1,temp.length);
										
										daycount = daycount + hournum ;
										var b = daycount;
									}
									absentForm.getForm().findField("actual_sum")
									.setValue(daycount);
							if(inputSeft!="" && inputSeft!=null){
									absentForm.getForm().findField("actual_sum")
									.setValue(inputSeft);
							}
					} else {
						Ext.Msg.alert(result.messageTitle, result.message);
					}

				},
				failure : function(response, action) {
					absentForm.getForm().findField("actual_sum").setValue("出错");
				}
			});
};
