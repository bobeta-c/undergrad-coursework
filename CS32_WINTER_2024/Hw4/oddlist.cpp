void removeOdd(list<int>& li)
{
	for(list<int>::iterator p = li.begin(); p != li.end(); p++){
		if(*p % 2 == 1){
			p = li.erase(p);
			p--;
		}
	}
}
