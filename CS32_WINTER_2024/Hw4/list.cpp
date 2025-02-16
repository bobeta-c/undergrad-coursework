void listAll(string path, const Domain* d) // two-parameter overload
{
    if(d->subdomains().size() == 0){
        cout << path << endl;
    }
    for(vector<Domain*>::const_iterator p = d->subdomains().begin(); p != d->subdomains().end(); p++){
        listAll(path != "" ? (*p)->label() + '.' + path : (*p)->label(), *p);
    }
}
