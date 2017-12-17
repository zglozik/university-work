 class FACTOR

creation {ANY}
   make

feature {ANY}
   
   make is 
      do  
      end; 
   
   compute(num: INTEGER): INTEGER is 
      do  
         if num = 1 then 
            Result := 1;
         else 
            Result := num * compute(num - 1);
         end; 
      end; 

end -- FACTOR

