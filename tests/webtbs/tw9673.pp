unit tw9673;

interface
{$mode objfpc}

type
  generic Testclass<T> = class
  
  type
    TList = array of byte;
	
  end;
	    
var
  b : specialize Testclass<LongInt>.TList;
	      
	    
implementation
begin
end.
