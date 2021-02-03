#pragma once

class SpinLock 
{
private:
  std::atomic<bool> _lock{false};

 public:
 	void lock() 
 	{
 		while (1) 
 		{
 			if (!_lock.exchange(true)) 
 				return;

      		while (_lock.load());
    	}
  	}

 	void unlock() 
 	{ 
 		_lock.store(false); 
 	}
};

/*for more info on spinlocks get ref from https://coffeebeforearch.github.io/2020/11/07/spinlocks-1.html*/

