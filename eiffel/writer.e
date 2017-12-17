 class WRITER

creation {ANY}
   make

feature {ANY}
   
   faktor: FACTOR;
   
   s1, s2: INTEGER is unique;
   
   cons: INTEGER is 100;
   
   make is 
      do  
         !!faktor.make;
         std_output.put_integer(faktor.compute(5));
         std_output.put_new_line;
      end; 
   
   printer is 
      do  
         std_output.put_integer(s1);
         std_output.put_new_line;
         std_output.put_integer(s2);
         std_output.put_new_line;
         std_output.put_integer(cons);
         std_output.put_new_line;
         std_output.put_string("Hello world!");
         std_output.put_new_line;
      end; 

end -- class WRITER
