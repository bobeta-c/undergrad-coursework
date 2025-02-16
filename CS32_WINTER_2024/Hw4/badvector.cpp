void removeBad(vector<Movie*>& v)
{
	for(vector<Movie*>::iterator p = v.begin(); p != v.end(); p++){
		if((*p)->rating() < 50){
			delete *p;
			p = v.erase(p);
			p--;
		}
	}
}
