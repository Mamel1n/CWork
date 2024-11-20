	Если возникает ошибка при "включении" UnitTest++.h,
необходимо выполнить команды:
	sudo ls /usr/include/*/
	sudo ls /usr/local/include/*/

	Если в каталоге */include содержится подкаталог UnitTest++,
необходимо использоать инструкцию включения:
	#include <UnitTest++/UnitTest++.h>

	Если в каталоге */include содержится подкаталог unittest++,
необходимо использоать инструкцию включения:
	#include <unittest++/UnitTest++.h>

	При отсутвии библиотеки, необходимо ее установить:
	sudo apt-get install cmake
	sudo apt-get install make
	sudo apt-get install git
	git clone https://github.com/unittest-cpp/unittest-cpp.git
	cd unittest-cpp
	mkdir build
	cd build
	cmake ..
	make
	sudo make install

	При возникновении ошибки на этапе make 
	- перейти по пути: /home/{имя пользователя}/unittest-cpp;
	- найти файл CMakeLists.txt;
	- закомментировать строку # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror");
	-выполнить комманды:
	make clean
	make
	sudo make install