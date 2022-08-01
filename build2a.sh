gcc -o autorhym -DFIR_FILT main4.c autorhym.c paUtils.c user_rio.c \
	-Wall \
	-I/usr/local/include \
	-L/usr/local/lib -lsndfile -lportaudio
	