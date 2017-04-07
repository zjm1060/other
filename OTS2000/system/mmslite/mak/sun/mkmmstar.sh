# ************************************************************************
# MODIFICATION LOG :	       						                     #
#  Date     Who   Rev			Comments			                     #
# --------  ---  ------   -------------------------------------------	 #
# 05/25/01  GLB    01     Created to build compressed mmslite files
# ************************************************************************

cd /

# create archive of mmslite files
tar -cvf ./mmslite.tar ./mmslite

# create compressed mmslite files from archived files
freeze mmslite.tar

# Note:	the above commands should produce the file:  mmslite.tar.F
#       in the root directory




		        