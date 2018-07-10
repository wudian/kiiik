
#include "WRServer.h"
#include "Baselib/Global.h"
#include <boost/filesystem.hpp>
#include <boost/progress.hpp>  
#include <boost/foreach.hpp> 

using namespace kiiik;



void copy_files(const boost::filesystem::path& from_dir, const boost::filesystem::path& to_dir, const std::string& filenames = "*")
{
	if (!boost::filesystem::exists(from_dir)) return;
	if (!boost::filesystem::exists(to_dir)) boost::filesystem::create_directory(to_dir);
	
	std::vector<std::string> file_vec;
	Split(filenames, ",", &file_vec);
	for (int i = 0; i < file_vec.size(); ++i){
		boost::filesystem::path from_file = from_dir / file_vec[i];
		boost::filesystem::path to_file = to_dir / file_vec[i];
		if (boost::filesystem::exists(to_file)) boost::filesystem::remove(to_file);
		if (boost::filesystem::exists(from_file)) boost::filesystem::copy(from_file, to_file);
	}	
}

int main(int argc, char *argv[]) {

	InitKiiik(argv);
	SetAppName("WarehouseReceiptSystem");

	/* 将新版本客户端copy到update目录中*/
	string kiiik_home = KiiikHome();
#ifdef _DEBUG
	copy_files(kiiik_home + "bin\\Debug", kiiik_home + "update", "Baselib.dll, NetworkAsio.dll, Timer.dll, WrUiClient.exe,VersionUpdate.exe");
#else
	copy_files(kiiik_home + "bin", kiiik_home + "update", "Baselib.dll, NetworkAsio.dll, Timer.dll, WrUiClient.exe,VersionUpdate.exe");
#endif // _DEBUG

	
	copy_files(kiiik_home + "config", kiiik_home + "update", "Client.xml,version.xml");
	//////////////////////////////////////////////////////////////////////////

	// 8863:test    8867
	WRServer *wr_server = new WRServer(8867);

	while(1){
		baselib::Thread::Sleep(30000000);
	}
}