public class Constants{


/****************************************weightings and constants***************************************************************/

public static int numberIterations = 10;

// NB All these constants are set to 1 for ease of debugging
/*
public static int spring_attack_weight = 1;
// Importance of defending our own centres in Spring
public static int spring_defence_weight = 1;

// Same for autumn.
public static int autumn_attack_weight = 1;
public static int autumn_defence_weight = 1;

// Importance of basicValue[0..n] in Spring
public static int[] spring_iteration_weight = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// Importance of our attack strength on a province in Spring
public static int spring_strength_weight = 1;

// Importance of lack of competition for the province in Spring
public static int spring_competition_weight = 1;

// Importance of basicValue[0..n] in autumn
public static int[] autumn_iteration_weight = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// Importance of our attack strength on a province in autumn
public static int autumn_strength_weight = 1;

// Importance of lack of competition for the province in autumn
public static int autumn_competition_weight = 1;

// Importance of building in provinces we need to defend
public static int build_defence_weight = 1;

// Importance of basicValue[0..n] when building
public static int[] build_iteration_weight = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
// Importance of removing in provinces we don't need to defend
public static int remove_defence_weight = 1;

// Importance of basicValue[0..n] when removing
public static int[] remove_iteration_weight = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// Percentage change of automatically playing the best move
public static int play_alternative = 1;

// If not automatic, chance of playing best move if inferior move is nearly as good
public static int alternative_difference_modifier = 1;
*/

 // Importance of attacking supply centres under enemy control in Spring
public static int spring_attack_weight = 70;
// Importance of defending our own centres in Spring
public static int spring_defence_weight = 30;

// Same for fall.
public static int autumn_attack_weight = 60;
public static int autumn_defence_weight = 40;

// The sum of the surrounding values is divided by this figure to work out an average
public static int iteration_fleet_divisor = 5;
public static int iteration_army_divisor = 5;

// Importance of basicValue[0..n] in Spring
public static int[] spring_iteration_weight =  {1000, 100, 30, 10, 6, 5, 4, 3, 2, 1};

// Importance of our attack strength on a province in Spring
public static int spring_strength_weight = 2;

// Importance of lack of competition for the province in Spring
public static int spring_competition_weight =  2;

public static int[] autumn_iteration_weight = {1000, 100, 30, 10, 6, 5, 4, 3, 2, 1};

// Importance of our attack strength on a province in Fall
public static int autumn_strength_weight = 2;

// Importance of lack of competition for the province in Fall
public static int autumn_competition_weight =  2;

// Importance of building in provinces we need to defend
public static int build_defence_weight = 1000;

// Importance of basicValue[0..n] when building
public static int[] build_iteration_weight = {1000, 100, 30, 10, 6, 5, 4, 3, 2, 1};
// Importance of removing in provinces we don't need to defend
public static int remove_defence_weight = 1000;

// Importance of basicValue[0..n] when removing
public static int[] remove_iteration_weight = {1000, 100, 30, 10, 6, 5, 4, 3, 2, 1};

// Percentage change of automatically playing the best move
public static int play_alternative = 50;

// If not automatic, chance of playing best move if inferior move is nearly as good
public static int alternative_difference_modifier = 500;
}
