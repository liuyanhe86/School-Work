x = 10;

while (x >= 10) {

	if (x < 30 && x > 20) {
		sum = sum + x;
	}
	else 
	{
		sum = sum - 5;
		x = x + 1;
	}
}

result = (sum - 100) * 2 - x / 2;
