void removeBad(list<Movie*>& li)
{
	for(list<Movie*>::iterator p = li.begin(); p != li.end(); p++){
		if((*p)->rating() < 50){
			delete *p;
			p = li.erase(p);
			p--;
		}
	}
}
