import { TreeNode } from "../TreeNode"
import { NodeAny } from "./NodeAny"

export class NodeString implements NodeAny{
    public key: string
    public pointer: TreeNode

    constructor(key: string, pointer: TreeNode) {
        this.key = key
        this.pointer = pointer
    }

    less(x: NodeString): boolean {
        return this.key < x.key
    }

    equal(x: NodeString): boolean {
        return this.key === x.key
    }
}