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
