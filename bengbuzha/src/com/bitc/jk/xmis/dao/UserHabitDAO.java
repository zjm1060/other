package com.bitc.jk.xmis.dao;

import com.bitc.jk.xmis.model.sys.UserHabit;

public interface UserHabitDAO {
	public UserHabit getUserHabit(int userID);

	public void doSaveUserHabit(UserHabit habit);

	public void doUpdateUserHabit(UserHabit habit);
}
