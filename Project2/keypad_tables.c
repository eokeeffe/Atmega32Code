short getButton(short port)
{
	// This function will take the input port
	// and return what button was pressed
	
	// follows the state machine architecture
	// for finding what button was pressed
	if(get_bit(port,5))
	{
		if(get_bit(port,1))
		{//Button Location 1
			return 1;
		}
		else if(get_bit(port,2))
		{//Button Location 4
			return 4;
		}
		else if(get_bit(port,3))
		{//Button Location 7
			return 7;
		}
		else if(get_bit(port,4))
		{//Button Location 10
			return 10;
		}

	}
	else if(get_bit(port,6))
	{
		if(get_bit(port,1))
		{//Button Location 2
			return 2;
		}
		else if(get_bit(port,2))
		{//Button Location 5
			return 5;
		}
		else if(get_bit(port,3))
		{//Button Location 8
			return 8;
		}
		else if(get_bit(port,4))
		{//Button Location 11
			return 11;
		}
	}
	else if(get_bit(port,7))
	{
		if(get_bit(port,1))
		{//Button Location 3
			return 3;
		}
		else if(get_bit(port,2))
		{//Button Location 6
			return 6;
		}
		else if(get_bit(port,3))
		{//Button Location 9
			return 9;
		}
		else if(get_bit(port,4))
		{//Button Location 12
			return 12;
		}
	}
	return 0;
}