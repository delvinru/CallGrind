using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Analyze
{
	public static class Analyze
	{
		public static void AddMarker(char* func_name, unsigned int g)
		{
			FileStream fstream =  new FileStream("output.txt", FileMode.OpenOrCreate);
			fstream.Write(func_name,)
		}
	}
}