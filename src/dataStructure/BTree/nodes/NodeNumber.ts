import { TreeNode } from "../TreeNode"
import { NodeAny } from "./NodeAny"

export class NodeNumber implements NodeAny{
    public key: number
    public pointer: TreeNode

    constructor(key: number, pointer: TreeNode) {
        this.key = key
        this.pointer = pointer
    }

    less(x: NodeNumber): boolean {
        return this.key < x.key
    }

    equal(x: NodeNumber): boolean {
        return this.key === x.key
    }
}