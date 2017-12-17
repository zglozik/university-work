-- Part of SmallEiffel -- Read DISCLAIMER file -- Copyright (C) 
-- Dominique COLNET and Suzanne COLLIN -- colnet@loria.fr
--
class TRY_PRINT_ON

creation 
   make, full_test
   
feature {ANY}
   
   cat: CAT;
   
   point: POINT;
   
   triangle: TRIANGLE;
   
   make is
      local
	 b8: BIT 8;
      do
	 std_output.put_string("a CHARACTER: ");
	 ('a').print_on(std_output);
	 std_output.put_new_line;
	 
	 std_output.put_string("a INTEGER: ");
	 (1).print_on(std_output);
	 std_output.put_new_line;
	 
	 std_output.put_string("a REAL: ");
	 (1.5).print_on(std_output);
	 std_output.put_new_line;
	 
	 std_output.put_string("a DOUBLE: ");
	 (1.5).to_double.print_on(std_output);
	 std_output.put_new_line;
	 
	 std_output.put_string("a STRING: ");
	 ("foo :-)").print_on(std_output);
	 std_output.put_new_line;

	 std_output.put_string("a BOOLEAN: ");
	 (true).print_on(std_output);
	 std_output.put_new_line;

	 std_output.put_string("a BIT 8: ");
	 b8 := 10101010B;
	 b8.print_on(std_output);
	 std_output.put_new_line;

	 std_output.put_string("print('a')=");
	 print('a');
	 std_output.put_new_line;

	 std_output.put_string("print(1)=");
	 print(1);
	 std_output.put_new_line;

	 std_output.put_string("print(1.5)=");
	 print(1.5);
	 std_output.put_new_line;

	 std_output.put_string("print((1.5).to_double)=");
	 print((1.5).to_double);
	 std_output.put_new_line;

	 std_output.put_string("print(%"foo%")=");
	 print("foo");
	 std_output.put_new_line;
      end;
   
   full_test is
	 -- Separated because it prints memory address and 
	 -- cause problems with SmallEiffel/misc/self_test.good_output
      do
	 !!cat;
	 std_output.put_string("a CAT: ");
	 cat.print_on(std_output);
	 std_output.put_new_line;
	 
	 !!point.make(1,2);
	 std_output.put_string("a POINT: ");
	 point.print_on(std_output);
	 std_output.put_new_line;
	 
	 !!triangle.make(point,point,clone(point));
	 std_output.put_string("a TRIANGLE: ");
	 triangle.print_on(std_output);
	 std_output.put_new_line;
	 
	 std_output.put_string("an ARRAY: ");
	 (<<'a','b','c'>>).print_on(std_output);
	 std_output.put_new_line;
	 
	 std_output.put_string("an ARRAY: ");
	 (<<1,2,3>>).print_on(std_output);
	 std_output.put_new_line;
	 
	 std_output.put_string("an ARRAY: ");
	 (<<"a","b","c">>).print_on(std_output);
	 std_output.put_new_line;
      end;
   
   is_true(b: BOOLEAN) is
      do
	 cpt := cpt + 1;
	 if not b then
	    std_output.put_string("TRY_PRINT_ON: ERROR Test # ");
	    std_output.put_integer(cpt);
	    std_output.put_string("%N");
	 else
	    std_output.put_string("Yes %N");
	 end;
      end;
   
   cpt: INTEGER;
   
end -- TRY_PRINT_ON
