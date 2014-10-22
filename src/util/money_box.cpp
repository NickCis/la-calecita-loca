#include "money_box.h"
#include "../util/env_config.h"
#include "../util/defines.h"

using std::unique_ptr;

MoneyBox::MoneyBox(){
	money = unique_ptr<MemoriaCompartida<int>> (new MemoriaCompartida<int>(Config::getBinPath(SELLER_BIN)));
	lock = unique_ptr<LockFile> (new LockFile(MONEY_BOX_LOCK));
}

MoneyBox::~MoneyBox(){
	lock->close();
}

int MoneyBox::read(){
	lock->tomarLock();
	int recaudacion = money->leer();
	lock->liberarLock();
	return recaudacion;
}

void MoneyBox::write(int val){
	money->escribir(val);
}

int MoneyBox::unlink(){
	return lock->unlink();
}
