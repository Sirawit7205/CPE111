package assign9;

import java.util.ArrayList;

public class HashNode{
    String word;                                        //word
    ArrayList<String> meaning = new ArrayList<>();      //meanings
    
    public HashNode() {}                                //constructor
    
    public HashNode(String input)                       //overloading constructor for new node
    {
        String [] buff = input.trim().replaceAll("\\s+"," ").toLowerCase().split(",");        //remove spaces + remove additional spaces + split with comma
        buff[1] = buff[1] + "(" + buff[2] + ")";        //concat type to meaning
        word = buff[0];                                 //add word to this node
        meaning.add(buff[1]);                           //add meaning to this node
    }
}
