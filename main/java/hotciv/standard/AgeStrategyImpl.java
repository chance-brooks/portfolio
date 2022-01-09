package hotciv.standard;

import hotciv.framework.AgeStrategy;
import hotciv.framework.GameConstants;

public class AgeStrategyImpl implements AgeStrategy {

    String civVersion;
    int age;

    public AgeStrategyImpl (String civVersion) {
        this.civVersion = civVersion;
        this.age = -4000;
    }

    public int ageWorld() {
        if (this.civVersion.equals(GameConstants.ALPHACIV))
            age += 100;

        else if (this.civVersion.equals(GameConstants.BETACIV)) {
            if (age >= -4000 && age < -100) age += 100;
            else if (age == -100) age = -1;
            else if (age == -1) age = 1;
            else if (age == 1) age = 50;
            else if (age >= 50 && age < 1750) age += 50;
            else if (age >= 1750 && age < 1900) age += 25;
            else if (age >= 1900 && age < 1970) age += 5;
            else age += 1;
        }

        return age;
    }

    //added because the age in GameImpl was incorrect until endofturn was called
    public int getAge(){
        return age;
    }


}
