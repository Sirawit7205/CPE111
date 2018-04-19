package assign8;

import java.util.ArrayList;

public class TreeNode implements Comparable<TreeNode>{
    String word;                                        //word
    ArrayList<String> meaning = new ArrayList<>();      //meanings
    
    public TreeNode() {}                                //constructor
    
    public TreeNode(String input)                       //overloading constructor for new node
    {
        String [] buff = input.trim().replaceAll("\\s+"," ").split(",");        //remove spaces + remove additional spaces + split with comma
        buff[1] = buff[1] + "(" + buff[2] + ")";        //concat type to meaning
        word = buff[0];                                 //set word
        meaning.add(buff[1]);                           //add meaning to this node
    }
    
    @Override
    public int compareTo(TreeNode o)                    //override compareTo
    {
        return (int) word.compareToIgnoreCase(o.word);  //compare with word only
    }
}
