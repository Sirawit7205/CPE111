package assign6;
import java.util.Scanner;

public class Assign6 {

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        String inputStr = new String();
        String [] splitStr = new String[100];
        boolean isNumber;
        int count=0;
        double num;
        
        while(inputStr.trim().compareToIgnoreCase("end")!=0)                        //if not "end" after trim
        {
            System.out.print("Enter text: ");
            inputStr=input.nextLine();                                              //get new line of data
            splitStr=inputStr.trim().split("\\s+");                                 //split with spaces
            
            for(int i=0;i<splitStr.length;i++)                                      //for all substrings
            {
                isNumber = true;                                                    //assume it is a number
                try {
                    num = Double.parseDouble(splitStr[i]);                          //try to convert to double
                }
                catch(NumberFormatException e) {
                    isNumber=false;                                                 //can't convert = not a number
                }
                
                if(splitStr.length==1&&splitStr[i].compareToIgnoreCase("end")==0)   //"end" case
                    System.out.println("End program");
                else if(isNumber)                                                               //number case
                    System.out.printf("%d[%s] => %s is Number%n",i,splitStr[i],splitStr[i]);
                else if(isFunction(splitStr[i]))                                                //function case
                    System.out.printf("%d[%s] => %s is Function%n",i,splitStr[i],splitStr[i]);
                else                                                                            //error case
                    System.out.printf("%d[%s] => %s is Error%n",i,splitStr[i],splitStr[i]);
            }
        }
        System.out.println("Program written by 60070501064 Sirawit Lappisatepun");
    }
    
    //function for checking if the string is a function name or not
    private static boolean isFunction(String text)
    {
        //all defined functions
        String [] functions = new String[]{"sin","cos","tan","asin","acos","atan","sqrt","log","exp","abs"};
        boolean ret = false;
        
        for(int i=0;i<functions.length;i++)                 //for each function name...
        {
            if(text.compareToIgnoreCase(functions[i])==0)   //if matched input string...
                ret=true;                                   //return true
        }
        return ret;                                         //return
    }
}
