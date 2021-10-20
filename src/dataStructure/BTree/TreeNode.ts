import { NodeAny } from "./nodes/NodeAny"

export class TreeNode{
    public father: TreeNode
    public list: Array<NodeAny>

    constructor(father: any){
        this.father = father
        this.list = new Array<any>()
    }

    public push(item: any): void {
        this.list.push(item)
    }    

    public static less(x: any, y: any): boolean {
        return x.less(y)
    }

    public static equal(x: any, y: any): boolean {
        return x.equal(y)
    }
}