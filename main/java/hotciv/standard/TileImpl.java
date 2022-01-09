package hotciv.standard;

import hotciv.framework.GameConstants;
import hotciv.framework.Position;
import hotciv.framework.Tile;

/** Tile represents a single territory tile of a given type.

 Responsibilities:
 1) Know its type.

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

public class TileImpl implements Tile {
    String tileType;

    public TileImpl(String tile_Type){
        tileType = tile_Type;
    }

    public String getTypeString() {
        return this.tileType;
    }

    public void setTileType(String type){
        tileType = type;
    }

    //TODO: Implement the production of the tiles, I.E. plains -> 3 food, oceans -> 1 food etc...
}
