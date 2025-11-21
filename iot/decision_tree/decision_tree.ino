# // DECISION TREE LOGIC
# // Feature mapping-> 0=Front, 1=Left, 2=Right
# // Classes-> 0=Forward, 1=Backward, 2=Right, 3=Left
int mlPredict(float distFront, float distLeft, float distRight) {
    if (distFront <= 19.91) {
        if (distLeft <= 28.44) {
            if (distRight <= 28.39) {
                //... more nested if-else rules
                return 1; // BACKWARD
            } else {
                //...
                return 2; // RIGHT
            }
        } else {
            //...
            return 3; // LEFT
        }
    } else {
        //...
        return 0; // FORWARD
    }
}
