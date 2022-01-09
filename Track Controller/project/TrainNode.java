package project;
import java.util.*;
import java.io.Serializable;

public class TrainNode implements Serializable {
    /**
     *
     */
    private static final long serialVersionUID = 6368785778083050635L;
    transient public Queue<Integer> AuthorityQueue;
    transient public Queue<Float[]> BlockQueue;
    transient public float SetpointSpeed;
    transient public int CrewCount;
    transient public boolean Line;
    transient public int NumOfCars;
    transient public String Key;

    public TrainNode(Queue<Integer> authority, Queue<Float[]> block, float setpoint_speed, int crew_count, boolean line_color, int cars, String key) {
        AuthorityQueue = authority;
        BlockQueue = block;
        SetpointSpeed = setpoint_speed;
        CrewCount = crew_count;
        Line = line_color;
        NumOfCars = cars;
        Key = key;
    }
}