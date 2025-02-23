double mean(const double* scores, int numScores)
    {
        const double* ptr = scores;
        double tot = 0;
	int location = 0;
        while (ptr + location != scores + numScores)
        {
            tot += *(ptr+location);
            location++;
        }
        return tot/numScores;
    }
