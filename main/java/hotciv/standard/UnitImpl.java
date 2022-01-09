package hotciv.standard;

import hotciv.framework.GameConstants;
import hotciv.framework.Player;
import hotciv.framework.Position;
import hotciv.framework.Unit;

/** Represents a single unit in the game.

 Responsibilities:
 1) Know its type name.
 2) Know its owner.
 2) Know its defensive and attacking strengths.

 This source code is from the book
 "Flexible, Reliable Software:
 Using Patterns and Agile Development"
 published 2010 by CRC Press.
 Author:
 Henrik B Christensen
 Department of Computer Science
 Aarhus University

 Please visit http://www.baerbak.com/ for further information.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 */

public class UnitImpl implements Unit {
    String unitType;
    Player unitOwner;
    Position unitPosition;
    int defensiveStrength;
    int attackingStrength;
    int moveCount;

    public UnitImpl(Position p, String kindOfUnit, Player ownerOfUnit){
        unitPosition = p;
        unitType = kindOfUnit;
        unitOwner = ownerOfUnit;
        if (kindOfUnit.equals(GameConstants.ARCHER)){
            defensiveStrength = 3;
            attackingStrength = 2;
            moveCount = 1;
        }
        if (kindOfUnit.equals(GameConstants.LEGION)) {
            defensiveStrength = 2;
            attackingStrength = 4;
            moveCount = 1;
        }
        if (kindOfUnit.equals(GameConstants.UFO)) {
            defensiveStrength = 8;
            attackingStrength = 1;
            moveCount = 2;
        }
        if (kindOfUnit.equals(GameConstants.SETTLER)) {
            defensiveStrength = 3;
            attackingStrength = 0;
            moveCount = 1;
        }
    }

    public void setUnitAttributes(Position p, String kindOfUnit, Player ownerOfUnit) {
        unitPosition = p;
        unitType = kindOfUnit;
        unitOwner = ownerOfUnit;
    }

    public void setUnitType(String set){
        unitType = set;
    }

    public void setUnitOwner(Player owner) { unitOwner = owner; }

    public void setUnitPosition(Position p) { unitPosition = p; }

    public String getTypeString(){
        return unitType;
    }

    public Position getUnitPosition(){
        return unitPosition;
    }

    public Player getOwner(){ return unitOwner; }

    public int getMoveCount() {return moveCount;}

    public void setMoveCount(int count){
        moveCount = count;
    }

    public void resetMoveCount(String type) {
        if (type == GameConstants.ARCHER) {
            moveCount = 1;
        }
        if (type == GameConstants.LEGION) {
            moveCount = 1;
        }
        if (type == GameConstants.SETTLER) {
            moveCount = 1;
        }
        if (type == GameConstants.UFO) {
            moveCount = 2;
        }
    }

    //Needed below 3 functions added in to avoid compiler error even though they are not implemented yet
    //Error was "... is not abstract and does not override abstract method ..."
    public int getAttackingStrength(){
        return attackingStrength;
    }

    public int getDefensiveStrength(){
        return defensiveStrength;
    }

    public void setAttackingStrength(int strength){
        attackingStrength = strength;
    }

    public void setDefensiveStrength(int strength){
        defensiveStrength = strength;
    }
}
