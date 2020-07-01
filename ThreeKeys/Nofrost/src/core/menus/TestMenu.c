timer_next_item_U16_  = 0;
timer_menu_U16 = now;
index = 0;
do
{
	switch(index)
	{
		case 0 :
			printSt();
			index |= WAIT_RELASE_KEYS;
			break;
		case 1 :
			printTa();
			break;
		case 2 :
			printTe();
			break;
		case 3 :
			printCo();
			break;
		case 4 :
			printFa();
			break;
		default :
			if(timer_next_item_U16_ > 1sec && timer_next_item_U16_)
			{
				timer_next_item_U16_ = 0;
				index |= WAIT_PRESS_KEYS;
			}
			if(timer_next_item_U16_ == 0)
			{
				if(index & WAIT_PRESS_KEYS)
				{
					if(key == Temp)
					{
						index &= ~WAIT_PRESS_KEYS;
						index++;
						if(index > 4)
							index = 1;
						timer_next_item_U16_ = now;
					}
					switch(index & ~WAIT_PRESS_KEYS)
					{
						case 0 :
							break;
						case 1:
							printTemp(Amb);
							break;
						case 2:
							printTemp(Eva);
							break;
						case 3 :
							printOn(Com);
							break;
						case 4 :
							printOn(Fan);
							break;
					}
				}
				if(index & WAIT_RELASE_KEYS && key == None)
				{
					index &= ~WAIT_RELASE_KEYS;
					index |= WAIT_PRESS_KEYS;
				}
			}
	}
	delay(100);
	key = getKey();
}while(timer_menu_U16 < 2minutes && ((index &  WAIT_RELASE_KEYS)  || (key != State)));